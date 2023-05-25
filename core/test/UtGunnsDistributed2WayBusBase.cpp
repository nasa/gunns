/*
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "UtGunnsDistributed2WayBusBase.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsDistributed2WayBusBase class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsDistributed2WayBusBase::UtGunnsDistributed2WayBusBase()
    :
    tArticle(0),
    tIsPairMaster(),
    tInData(),
    tOutData()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsDistributed2WayBusBase class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsDistributed2WayBusBase::~UtGunnsDistributed2WayBusBase()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDistributed2WayBusBase::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDistributed2WayBusBase::setUp()
{
    tIsPairMaster = true;
    tArticle      = new FriendlyGunnsDistributed2WayBusBase(&tInData, &tOutData);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDistributed2WayBusBase::testDefaultConstruction()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsDistributed2WayBusBase 01: testDefaultConstruction ..........";

    /// @test state data
    CPPUNIT_ASSERT(false                             == tArticle->mIsPairMaster);
    CPPUNIT_ASSERT(false                             == tArticle->mInDataLastDemandMode);
    CPPUNIT_ASSERT(0                                 == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(0                                 == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(GunnsDistributed2WayBusBase::NONE == tArticle->mForcedRole);
    CPPUNIT_ASSERT(0                                 == tArticle->mNotifications.size());

    /// @test new/delete for code coverage
    GunnsDistributed2WayBusBase* article = new GunnsDistributed2WayBusBase(&tInData, &tOutData);
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

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the initialization method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDistributed2WayBusBase::testInitialization()
{
    std::cout << "\n UtGunnsDistributed2WayBusBase 02: testInitialization ...............";

    tArticle->initialize(tIsPairMaster);

    CPPUNIT_ASSERT(tIsPairMaster == tArticle->mIsPairMaster);
    CPPUNIT_ASSERT(false         == tArticle->mInDataLastDemandMode);
    CPPUNIT_ASSERT(0             == tArticle->mFramesSinceFlip);
    CPPUNIT_ASSERT(0             == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(false         == tOutData.mDemandMode);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the updateFrameCounts method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDistributed2WayBusBase::testFrameCounts()
{
    std::cout << "\n UtGunnsDistributed2WayBusBase 03: testFrameCounts ..................";

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
    CPPUNIT_ASSERT(expectedLoopLatency   == tArticle->mLoopLatency);
    CPPUNIT_ASSERT(tInData.mFrameCount   == tOutData.mFrameLoopback);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the push and pop notifications methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDistributed2WayBusBase::testNotifications()
{
    std::cout << "\n UtGunnsDistributed2WayBusBase 04: testNotifications ................";

    tArticle->initialize(tIsPairMaster);

    /// @test pushNotification
    const std::string message1 = "1test1";
    tArticle->pushNotification(GunnsDistributed2WayBusNotification::ERR, message1);
    CPPUNIT_ASSERT(1 == tArticle->mNotifications.size());

    const std::string message2 = "2test2";
    tArticle->pushNotification(GunnsDistributed2WayBusNotification::INFO, message2);
    CPPUNIT_ASSERT(2 == tArticle->mNotifications.size());

    /// @test popNotification
    GunnsDistributed2WayBusNotification notif;
    unsigned int numMessages = tArticle->popNotification(notif);
    CPPUNIT_ASSERT(1 == numMessages);
    CPPUNIT_ASSERT(1 == tArticle->mNotifications.size());
    CPPUNIT_ASSERT(0 == notif.mMessage.rfind("2test2", 0));
    CPPUNIT_ASSERT(GunnsDistributed2WayBusNotification::INFO == notif.mLevel);

    numMessages = tArticle->popNotification(notif);
    CPPUNIT_ASSERT(0 == numMessages);
    CPPUNIT_ASSERT(0 == tArticle->mNotifications.size());
    CPPUNIT_ASSERT(0 == notif.mMessage.rfind("1test1", 0));
    CPPUNIT_ASSERT(GunnsDistributed2WayBusNotification::ERR == notif.mLevel);

    numMessages = tArticle->popNotification(notif);
    CPPUNIT_ASSERT(0 == numMessages);
    CPPUNIT_ASSERT(0 == tArticle->mNotifications.size());
    CPPUNIT_ASSERT(0 == notif.mMessage.rfind("", 0));
    CPPUNIT_ASSERT(GunnsDistributed2WayBusNotification::NONE == notif.mLevel);

    /// @test assignment operator for code coverage
    GunnsDistributed2WayBusNotification notif2(GunnsDistributed2WayBusNotification::WARN, "notif2");
    notif = notif2;
    CPPUNIT_ASSERT(0 == notif.mMessage.rfind("notif2", 0));
    CPPUNIT_ASSERT(GunnsDistributed2WayBusNotification::WARN == notif.mLevel);

    /// @test self assign
    notif = notif;
    CPPUNIT_ASSERT(0 == notif.mMessage.rfind("notif2", 0));
    CPPUNIT_ASSERT(GunnsDistributed2WayBusNotification::WARN == notif.mLevel);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the mode forcing flags.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDistributed2WayBusBase::testAccessors()
{
    std::cout << "\n UtGunnsDistributed2WayBusBase 05: testAccessors ....................";

    /// @test isInDemandRole
    tArticle->initialize(tIsPairMaster);
    CPPUNIT_ASSERT(false == tArticle->isInDemandRole());
    tOutData.mDemandMode = true;
    CPPUNIT_ASSERT(true  == tArticle->isInDemandRole());

    /// @test forceSupplyRole
    tArticle->forceSupplyRole();
    CPPUNIT_ASSERT(GunnsDistributed2WayBusBase::SUPPLY == tArticle->mForcedRole);

    /// @test forceSupplyRole
    tArticle->forceDemandRole();
    CPPUNIT_ASSERT(GunnsDistributed2WayBusBase::DEMAND == tArticle->mForcedRole);

    /// @test resetForceRole
    tArticle->resetForceRole();
    CPPUNIT_ASSERT(GunnsDistributed2WayBusBase::NONE   == tArticle->mForcedRole);

    std::cout << "... Pass";
}
